#pragma once

#include <memory>
#include <string_view>
#include <unordered_map>

#include <plg/string.hpp>
#include <plg/hash.hpp>

namespace pcf {
	class Config;

	class ReaderFactory {
	public:
		using ReaderLoad = std::unique_ptr<Config> (*)(std::string_view path);

		void RegisterReader(std::string_view ext, ReaderLoad func);
		std::unique_ptr<Config> ReadConfig(std::string_view path);

	private:
		std::unordered_map<std::string, ReaderLoad, plg::string_hash, std::equal_to<>> _readers;
	};
}// namespace pcf

