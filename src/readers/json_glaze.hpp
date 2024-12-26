#pragma once
#include <string_view>
#include <memory>

namespace pcf
{
	class Config;

	std::unique_ptr<Config> ReadJsonConfig(std::string_view path);
}
