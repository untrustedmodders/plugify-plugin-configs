#include "config.hpp"

namespace pcf
{
	Config::Detail::Node::Node() = default;

	std::unique_ptr<Config> Config::Detail::MakeConfig()
	{
		return std::unique_ptr<Config>(new Config(std::unique_ptr<Detail>(new Detail())));
	}

	Config::Detail::Detail() = default;

	void Config::Set(int64_t value)
	{
		_detail->Set(value);
	}

	Config::Config(std::unique_ptr<Config::Detail> detail)
		: _detail(std::move(detail))
	{
	}

	Config::~Config() = default;
}
