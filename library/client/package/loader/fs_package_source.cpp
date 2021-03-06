#include "includes.hpp"
#include "fs_package_source.hpp"

#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>


namespace vps {


FsPackageSource::FsPackageSource( const fs::path& path )
	: mPath(fs::canonical(path))
{
	assert(fs::is_directory(path));
}

FsPackageSource::FsPackageSource( const fs::path& path, const shared_ptr<FsPackageSource>& root )
	: mPath(path)
	, mRoot(root)
{
	assert(fs::is_directory(path));
}

OVERRIDE shared_ptr<VirtualPackageSource> FsPackageSource::root()
{
	return mRoot? mRoot : shared_from_this();
}

OVERRIDE shared_ptr<VirtualPackageSource> FsPackageSource::dir( const string& path )
{
	fs::path p = buildPath(path);

	if(!fs::is_directory(p))
	{
		BOOST_THROW_EXCEPTION(excp::VPSExpection() << excp::InfoWhat(L"Failed access directory[" + path + L"]"));
		return nullptr;
	}

	return std::make_shared<FsPackageSource>(p, std::static_pointer_cast<FsPackageSource>(shared_from_this()));
}

OVERRIDE shared_ptr<VirtualPackageSource> FsPackageSource::loadVPS( const string& path )
{
	return LoadVPSFromSF(buildPath(path));
}

OVERRIDE std::vector<wchar_t> FsPackageSource::loadText( const string& path )
{
	fs::path p = buildPath(path);

	std::wifstream stream(p.native());
	stream >> std::noskipws;

	if(!stream)
	{
		BOOST_THROW_EXCEPTION(excp::VPSExpection() << excp::InfoWhat(L"Failed to access virtual source[" + path + L"]"));
	}

	std::vector<wchar_t> buffer;
	std::copy(	std::istream_iterator<wchar_t, wchar_t>(stream),
				std::istream_iterator<wchar_t, wchar_t>(),
				std::back_inserter(buffer));
	buffer.push_back(0);

	if(stream.bad())
	{
		BOOST_THROW_EXCEPTION(excp::VPSExpection() << excp::InfoWhat(L"Error occured while loading virtual source[" + path + L"]"));
	}

	return buffer;
}

OVERRIDE std::vector<byte> FsPackageSource::loadRaw( const string& path )
{
	fs::path p = buildPath(path);

	std::ifstream stream(p.string(), std::ios::in | std::ios::binary);

	boost::system::error_code ec;
	auto size = fs::file_size(p, ec);

	if(!stream || ec)
	{
		BOOST_THROW_EXCEPTION(excp::VPSExpection() << excp::InfoWhat(L"Failed to access virtual source[" + path + L"]"));
	}

	std::vector<byte> buffer;
	buffer.resize(size);
	stream.read((char*)buffer.data(), size);
	buffer.push_back(0);

	if(stream.bad() || stream.gcount() != size)
	{
		BOOST_THROW_EXCEPTION(excp::VPSExpection() << excp::InfoWhat(L"Error occured while loading virtual source[" + path + L"]"));
	}


	return buffer;
}

OVERRIDE bool FsPackageSource::isRoot()
{
	return mRoot != nullptr;
}

OVERRIDE bool FsPackageSource::isValidPath( const string& path )
{
	return fs::exists(buildPath(path));
}

fs::path FsPackageSource::buildPath( const string& path )
{
	fs::path result;

	if(path.front() == L'/')
	{
		if(mRoot)
			return mRoot->buildPath(path.substr(1));

		result = mPath / path.substr(1);
	}else{
		result = mPath / path;
	}

	boost::system::error_code ec;
	result = fs::canonical(result, ec);

	if(ec || result.string().find(mPath.string()) == string::npos)
	{
		BOOST_THROW_EXCEPTION(excp::VPSExpection() << excp::InfoWhat(L"Access denied[" + result.wstring() + L"]"));
	}

	assert(result.is_absolute());

	return result;
}

OVERRIDE string FsPackageSource::srcname()
{
	return mPath.wstring();
}

}