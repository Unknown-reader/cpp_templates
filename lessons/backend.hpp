// Общий домен для всех уроков — как минимальный бэкенд-фреймворк.
//
// Чтобы примеры были прикладными, а не абстрактными, каждый урок работает с
// одними и теми же сущностями: HTTP-запрос/ответ, статус, пользователь и
// ошибка доступа к БД. В настоящем сервисе эти типы жили бы в отдельном модуле.

#pragma once

#include <map>
#include <string>
#include <string_view>

namespace backend {

enum class HttpStatus {
    Ok = 200,
    Created = 201,
    BadRequest = 400,
    NotFound = 404,
    InternalError = 500,
};

enum class Method { Get, Post, Put, Delete };

inline std::string_view method_name(Method method) {
    switch (method) {
        case Method::Get:    return "GET";
        case Method::Post:   return "POST";
        case Method::Put:    return "PUT";
        case Method::Delete: return "DELETE";
    }
    return "?";
}

struct Request {
    Method method = Method::Get;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;

    std::string_view header(std::string_view name) const {
        auto it = headers.find(std::string{name});
        return it == headers.end() ? std::string_view{} : std::string_view{it->second};
    }
};

struct Response {
    HttpStatus status = HttpStatus::Ok;
    std::string content_type = "text/plain";
    std::string body;
    std::map<std::string, std::string> headers;
};

struct User {
    int id = 0;
    std::string name;
    std::string email;
};

struct DbError {
    std::string message;
};

}  // namespace backend
