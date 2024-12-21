#pragma once
#include <plugify/string.hpp>
#include <map>
#include <string_view>
#include <memory>

namespace pcf
{
	class Config;

	class ReaderFactory
	{
	public:
		using ReaderLoad = std::unique_ptr<Config> (*)(std::string_view path);

		void RegisterReader(std::string_view ext, ReaderLoad func);
		std::unique_ptr<Config> ReadConfig(std::string_view path);

	private:
		std::map<plg::string, ReaderLoad> _readers;
	};
}
