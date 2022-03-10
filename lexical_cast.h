#ifndef LEXICAL_CAST_H
#define LEXICAL_CAST_H
#include <type_traits>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <cstring>
using namespace std;

namespace internal
{
    const char* strue = "true";
    const char* sfalse = "false";

    template <typename To, typename From>
    struct Converter
    {
    };

    //to numeric
    template <typename From>
    struct Converter<int, From>
    {
        static int convert(const From& from)
        {
            return std::atoi(from);
        }
    };

    template <typename From>
    struct Converter<long, From>
    {
        static long convert(const From& from)
        {
            return std::atol(from);
        }
    };

    template <typename From>
    struct Converter<long long, From>
    {
        static long long convert(const From& from)
        {
            return std::atoll(from);
        }
    };

    template <typename From>
    struct Converter<double, From>
    {
        static double convert(const From& from)
        {
            return std::atof(from);
        }
    };

    template <typename From>
    struct Converter<float, From>
    {
        static float convert(const From& from)
        {
            return (float)std::atof(from);
        }
    };

    //to bool
    template <typename From>
    struct Converter<bool, From>
    {
        static typename std::enable_if<std::is_integral<From>::value, bool>::type convert(From from)
        {
            return !!from;
        }
    };

    bool checkbool(const char* from, const size_t len, const char* s)
    {
        for (size_t i = 0; i < len; i++)
        {
            if (from[i] != s[i])
            {
                return false;
            }
        }

        return true;
    }

    static bool convertbool(const char* from)
    {
        const unsigned int len = strlen(from);
        if (len != 4 && len != 5)
            throw std::invalid_argument("argument is invalid");

        bool r = true;
        if (len == 4)
        {
            r = checkbool(from, len, strue);

            if (r)
                return true;
        }
        else
        {
            r = checkbool(from, len, sfalse);

            if (r)
                return false;
        }

        throw std::invalid_argument("argument is invalid");
    }

    template <>
    struct Converter<bool, string>
    {
        static bool convert(const string& from)
        {
            return internal::convertbool(from.c_str());
        }
    };

    template <>
    struct Converter<bool, const char*>
    {
        static bool convert(const char* from)
        {
            return internal::convertbool(from);
        }
    };

    template <>
    struct Converter<bool, char*>
    {
        static bool convert(char* from)
        {
            return internal::convertbool(from);
        }
    };

    template <unsigned N>
    struct Converter<bool, const char[N]>
    {
        static bool convert(const char(&from)[N])
        {
            return internal::convertbool(from);
        }
    };

    template <unsigned N>
    struct Converter<bool, char[N]>
    {
        static bool convert(const char(&from)[N])
        {
            return internal::convertbool(from);
        }
    };

    //to string
    template <typename From>
    struct Converter<string, From>
    {
        static string convert(const From& from)
        {
            return std::to_string(from);
        }
    };
}

template <typename To, typename From>
typename std::enable_if<!std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
{
    return internal::Converter<To, From>::convert(from);
}

template <typename To, typename From>
typename std::enable_if<std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
{
    return from;
}


#endif // LEXICAL_CAST_H
