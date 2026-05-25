#pragma once
#include <iostream>
#include <cmath>
#include <functional>
#include <cstdlib>

inline int& passed() { static int v = 0; return v; }
inline int& failed() { static int v = 0; return v; }

inline void check_bool(bool cond, const char* desc, int line) {
    if (cond) { std::cout << "  [OK] " << desc << "\n"; passed()++; }
    else { std::cout << "  [FAIL] " << desc << " at line " << line << "\n"; failed()++; }
}

// 🔹 Исправлено: передача по ЗНАЧЕНИУ. Это обходит 99% ошибок дедукции в MSVC
template<typename T, typename U>
inline void check_eq(T a, U b, const char* desc, int line) {
    if (a == b) { std::cout << "  [OK] " << desc << "\n"; passed()++; }
    else { std::cout << "  [FAIL] " << desc << " at line " << line << "\n"; failed()++; }
}

inline void check_near(double a, double b, double eps, const char* desc, int line) {
    if (std::abs(a - b) < eps) { std::cout << "  [OK] " << desc << "\n"; passed()++; }
    else { std::cout << "  [FAIL] " << desc << " at line " << line << " | Exp: " << b << ", Got: " << a << "\n"; failed()++; }
}

inline void check_throw(std::function<void()> func, const char* desc, int line) {
    bool thrown = false;
    try { func(); }
    catch (...) { thrown = true; }
    if (thrown) { std::cout << "  [OK] " << desc << "\n"; passed()++; }
    else { std::cout << "  [FAIL] " << desc << " at line " << line << "\n"; failed()++; }
}

#define CHECK(cond) check_bool((cond), #cond, __LINE__)
#define CHECK_EQ(a, b) check_eq((a), (b), #a " == " #b, __LINE__)
#define CHECK_NEAR(a, b, eps) check_near((a), (b), (eps), #a " ≈ " #b, __LINE__)
#define CHECK_THROW(expr) check_throw([&]{ expr; }, #expr " throws", __LINE__)