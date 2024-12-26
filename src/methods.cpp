#include "plugin.hpp"
#include "config.hpp"
#include <string_view>
#include <memory>

namespace pcf
{
	extern ConfigsPlugin plugin;
}

std::unique_ptr<pcf::Config> ReadConfig(std::string_view path)
{
	return pcf::plugin.ReadConfig(path);
}

std::unique_ptr<pcf::Config> MakeConfig()
{
	return pcf::Config::Detail::MakeConfig();
}
