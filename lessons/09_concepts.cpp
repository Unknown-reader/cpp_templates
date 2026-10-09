// Урок 9. Концепты в бэкенде.
//
// Концепты описывают требования к доменным типам словами: «сущность с id»,
// «репозиторий с save/find», «сериализатор». Ошибка видна прямо в сигнатуре,
// а не в глубине инстанциации.

#include <concepts>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "backend.hpp"

namespace {

// Сущность — тип с числовым полем id.
template <typename T>
concept Entity = requires(const T& entity) {
    { entity.id } -> std::convertible_to<int>;
};

// Репозиторий — умеет сохранять и искать по id свою сущность.
template <typename T>
concept Repository = requires(T repo, const typename T::entity_type& entity) {
    { repo.save(entity) } -> std::same_as<void>;
    { repo.find(entity.id) }
        -> std::convertible_to<std::optional<typename T::entity_type>>;
};

// Сериализатор — статический serialize для нужного типа.
template <typename S, typename T>
concept Serializer = requires(const T& value) {
    { S::serialize(value) } -> std::convertible_to<std::string>;
};

// Конкретный репозиторий и сериализатор из домена backend.
class UserRepository {
public:
    using entity_type = backend::User;

    void save(const backend::User& user) { data_.push_back(user); }

    std::optional<backend::User> find(int id) const {
        for (const auto& user : data_) {
            if (user.id == id) return user;
        }
        return std::nullopt;
    }

private:
    std::vector<backend::User> data_;
};

struct UserSerializer {
    static std::string serialize(const backend::User& user) {
        return "{\"id\":" + std::to_string(user.id) + "}";
    }
};

// Обобщённые функции, ограниченные концептами.
template <Entity T>
std::string resource_name(const T& entity) {
    return "resource/" + std::to_string(entity.id);
}

template <Repository Repo>
void register_user(Repo& repo, const backend::User& user) {
    repo.save(user);
}

void run() {
    const backend::User alice{1, "Алиса", "alice@example.com"};

    std::cout << resource_name(alice) << '\n';

    UserRepository repository;
    register_user(repository, alice);
    if (auto user = repository.find(1)) {
        std::cout << "сохранён: " << user->name << '\n';
    }

    std::cout << std::boolalpha
              << "Entity<User>:              " << Entity<backend::User> << '\n'
              << "Repository<UserRepository>: " << Repository<UserRepository> << '\n'
              << "Serializer<User>:           "
              << Serializer<UserSerializer, backend::User> << '\n';

    static_assert(Entity<backend::User>);
    static_assert(Repository<UserRepository>);
    static_assert(Serializer<UserSerializer, backend::User>);
    static_assert(!Entity<std::string>);
}

}  // namespace

int main() { run(); }
