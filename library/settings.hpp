#pragma once
#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

/// @cond DEV

#include <assert.h>
#include <string>
#include <memory>
#include <functional>

#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

#include "tilenet.h"

#define OVERRIDE

typedef unsigned int IdType;
typedef std::wstring string;

using std::shared_ptr;
using std::weak_ptr;

#ifdef _DEBUG
#	define TILENET_DEBUG
#endif


#define NOT_IMPLEMENTED()	{assert(!"Not implemented"); BOOST_THROW_EXCEPTION(excp::NotImplException());}
#define IMPLEMENTATION_TODO(_text)	{																\
										static bool BOOST_JOIN(_found_, __LINE__) = false;			\
										if(! BOOST_JOIN(_found_, __LINE__))							\
										{															\
											std::wclog << "Not Implemented: "<< _text << std::endl;	\
											BOOST_JOIN(_found_, __LINE__) = true;					\
										}															\
									}


#ifdef TILENET_USE_ASSOZIATIVE_EMPLACE
#	define TILENET_EMPLACE_PAIR(_c, _p)					(_c).emplace(_p)
#	define TILENET_EMPLACE(_c, _key, ...)				(_c).emplace(_key, __VA_ARGS__)
#	define TILENET_EMPLACE_HINT(_c, _hint, _key, ...)	(_c).emplace_hint(_hint, _key, __VA_ARGS__)
#else
#	define TILENET_EMPLACE_PAIR(_c, _p)					(_c).insert(_p)
#	define TILENET_EMPLACE(_c, _key, ...)				(_c).insert(std::make_pair(_key, std::tuple_element<1, std::remove_reference<decltype(_c)>::type::value_type>::type(__VA_ARGS__)))
#	define TILENET_EMPLACE_HINT(_c, _hint, _key, ...)	(_c).insert(_hint, std::make_pair(_key, std::tuple_element<1, std::remove_reference<decltype(_c)>::type::value_type>::type(__VA_ARGS__)))
#endif



#define tnAssert(_expr)		assert(_expr)
#define tilenet_vcast		dynamic_cast

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef uint8 byte;


typedef uint8 msgid_type;
typedef uint32 version_type;


typedef struct {

} TilenetNullData;



#include "utils/exceptions.hpp"
#include "utils/tilenet_object.hpp"
#include "utils/lexical_convert.hpp"
#include "utils/vector.hpp"
#include "utils/log.hpp"
#include "utils/additional_hashs.hpp"


typedef basic_vector2<unsigned int> Point;
typedef basic_vector2<unsigned int> Rect;
typedef basic_vector2<TNRATIO>		Ratio;
typedef basic_vector2<float>		Vector;
typedef Ratio						Aspect;


/// @endcond

#endif
