#pragma once

#include <functional>
#include <iostream>
#include <cassert>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace Support {

    class Options {
    public:
        // Define the type of "Action";
        typedef std::function<void(Options *, const std::string& )> Action;
        enum class Arguments { Zero, One, N, Optional };

        std::map<std::string, std::string> extra;

        Options(const std::string& command, const std::string& description);
        ~Options();
        Options &add(const std::string& longName, const std::string& shortName,
                     const std::string& description, Arguments arguments,
                     const Action &action);
        Options &add_positional(const std::string& name, Arguments arguments,
                                const Action &action);
        void parse(int argc, const char *argv[]);

    private:
        Options() = delete;
        Options(const Options &) = delete;
        Options &operator=(const Options &) = delete;

        struct Option {
            std::string longName;
            std::string shortName;
            std::string description;
            Arguments arguments;
            Action action;
            size_t seen;
        };
        std::vector<Option> options;
        Arguments positional;
        std::string positionalName;
        Action positionalAction;
    };

}  // namespace wasm