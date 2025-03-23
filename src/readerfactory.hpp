#pragma once

#include <plugify/string.hpp>
#include <unordered_map>
#include <string_view>
#include <memory>

namespace pcf
{
	class Config;

    struct string_hash {
        using is_transparent = void;
        [[nodiscard]] size_t operator()(const char* txt) const {
            return std::hash<std::string_view>{}(txt);
        }

        [[nodiscard]] size_t operator()(std::string_view txt) const {
            return std::hash<std::string_view>{}(txt);
        }

        [[nodiscard]] size_t operator()(const std::string& txt) const {
            return std::hash<std::string>{}(txt);
        }
    };

    class ReaderFactory
	{
	public:
		using ReaderLoad = std::unique_ptr<Config> (*)(std::string_view path);

		void RegisterReader(std::string_view ext, ReaderLoad func);
		std::unique_ptr<Config> ReadConfig(std::string_view path);

	private:
        std::unordered_map<std::string, ReaderLoad, string_hash, std::equal_to<>> _readers;
	};
}
