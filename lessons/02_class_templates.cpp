// Урок 2. Шаблоны классов в бэкенде.
//
// Хендлеру нужно вернуть либо данные, либо ошибку. Обёртка Result<T, E> хранит
// оба варианта в одном типе, а Cache<K, V> и Repository<Entity> параметризуются
// типами ключа и сущности — один код на все сущности сервиса.

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "backend.hpp"

namespace {

// Результат операции: значение или ошибка. Упрощённый std::expected.
template <typename T, typename E>
class Result {
public:
    using value_type = T;
    using error_type = E;

    static Result ok(T value) { return Result{std::move(value)}; }
    static Result fail(E error) { return Result{std::move(error)}; }

    bool has_value() const { return value_.has_value(); }
    explicit operator bool() const { return has_value(); }

    const T& value() const { return *value_; }
    const E& error() const { return error_; }

private:
    explicit Result(T value) : value_(std::move(value)) {}
    explicit Result(E error) : error_(std::move(error)) {}

    std::optional<T> value_;
    E error_{};
};

// Кэш: и ключ, и значение — любые типы. Здесь ключ — id пользователя.
template <typename K, typename V>
class Cache {
public:
    void put(const K& key, V value) { data_[key] = std::move(value); }

    const V* get(const K& key) const {
        auto it = data_.find(key);
        return it == data_.end() ? nullptr : &it->second;
    }

    void invalidate(const K& key) { data_.erase(key); }
    std::size_t size() const { return data_.size(); }

private:
    std::map<K, V> data_;
};

// Репозиторий поверх кэша: тип сущности — параметр шаблона.
template <typename Entity, typename Id>
class Repository {
public:
    explicit Repository(Cache<Id, Entity>& cache) : cache_(cache) {}

    void save(const Id& id, Entity entity) { cache_.put(id, std::move(entity)); }

    std::optional<Entity> find(const Id& id) const {
        if (const Entity* entity = cache_.get(id)) return *entity;
        return std::nullopt;
    }

private:
    Cache<Id, Entity>& cache_;
};

void run() {
    using backend::DbError;
    using backend::User;

    Cache<int, User> users;
    Repository<User, int> repository{users};
    repository.save(1, User{1, "Алиса", "alice@example.com"});

    if (auto user = repository.find(1)) {
        std::cout << "найден: " << user->name << " <" << user->email << ">\n";
    }

    std::cout << std::boolalpha
              << "find(42) есть значение: " << repository.find(42).has_value()
              << ", в кэше: " << users.size() << " записи\n";

    // Один и тот же Result работает с данными и с ошибкой БД.
    auto ok = Result<User, DbError>::ok(User{2, "Боб", "bob@example.com"});
    auto fail = Result<User, DbError>::fail(DbError{"нет соединения с БД"});

    std::cout << "ok: " << ok.has_value()
              << ", fail: " << fail.has_value()
              << ", причина: " << fail.error().message << '\n';

    static_assert(std::is_same_v<decltype(ok)::value_type, User>);
}

}  // namespace

int main() { run(); }
