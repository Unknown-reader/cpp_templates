// Урок 10. CRTP, policy-based design и вычисления на этапе компиляции.
//
// CRTP даёт общий REST-контроллер поверх разных хранилищ без виртуальных
// вызовов, политики подставляют retry и логирование в клиент БД, а хеш маршрута
// считается компилятором.

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "backend.hpp"

namespace {

// --- CRTP: базовый контроллер знает точный тип наследника через параметр.
template <typename Derived, typename Entity>
class ResourceController {
public:
    backend::Request get(int id) const {
        backend::Request request;
        request.method = backend::Method::Get;
        request.path =
            "/" + std::string{Derived::route()} + "/" + std::to_string(id);
        return request;
    }

    std::optional<Entity> fetch(int id) const {
        return static_cast<const Derived*>(this)->load(id);
    }
};

class UserController : public ResourceController<UserController, backend::User> {
public:
    static constexpr std::string_view route() { return "users"; }

    std::optional<backend::User> load(int id) const {
        if (id == 1) return backend::User{1, "Алиса", "alice@example.com"};
        return std::nullopt;
    }
};

// --- Policy-based design: поведение задаётся параметрами шаблона.
struct NoRetry { static constexpr int attempts = 1; };
struct RetryThreeTimes { static constexpr int attempts = 3; };

struct NoLog {
    static void log(std::string_view) {}
};
struct StdoutLog {
    static void log(std::string_view message) {
        std::cout << "[db] " << message << '\n';
    }
};

template <typename RetryPolicy, typename LogPolicy>
class DatabaseClient {
public:
    void connect(std::string_view dsn) const {
        LogPolicy::log("connect " + std::string{dsn});
        for (int attempt = 1; attempt <= RetryPolicy::attempts; ++attempt) {
            LogPolicy::log("attempt " + std::to_string(attempt));
        }
    }

    static constexpr int max_attempts() { return RetryPolicy::attempts; }
};

// --- Вычисления на этапе компиляции: FNV-1a хеш маршрута.
constexpr std::uint32_t fnv1a(std::string_view text) {
    std::uint32_t hash = 2166136261u;
    for (char ch : text) {
        hash ^= static_cast<std::uint32_t>(static_cast<unsigned char>(ch));
        hash *= 16777619u;
    }
    return hash;
}

// Маршрут, узнающий себя по хешу пути без сравнения строк.
template <std::uint32_t Hash>
struct Route {
    static constexpr std::uint32_t hash = Hash;
    static constexpr bool matches(std::string_view path) { return fnv1a(path) == Hash; }
};

using UsersRoute = Route<fnv1a("/users")>;
using HealthRoute = Route<fnv1a("/health")>;

void run() {
    UserController controller;
    const backend::Request request = controller.get(1);
    std::cout << backend::method_name(request.method) << " " << request.path << '\n';

    if (auto user = controller.fetch(1)) {
        std::cout << "user: " << user->name << '\n';
    }

    DatabaseClient<RetryThreeTimes, StdoutLog> client;
    client.connect("postgres://localhost/app");
    std::cout << "max attempts: " << client.max_attempts() << '\n';

    std::cout << std::boolalpha
              << "GET /users -> users:  " << UsersRoute::matches("/users") << '\n'
              << "GET /users -> health: " << UsersRoute::matches("/health") << '\n';

    static_assert(DatabaseClient<RetryThreeTimes, NoLog>::max_attempts() == 3);
    static_assert(DatabaseClient<NoRetry, NoLog>::max_attempts() == 1);
    static_assert(UsersRoute::matches("/users"));
    static_assert(!UsersRoute::matches("/health"));
    static_assert(UsersRoute::hash == fnv1a("/users"));
    static_assert(HealthRoute::hash != UsersRoute::hash);
}

}  // namespace

int main() { run(); }
