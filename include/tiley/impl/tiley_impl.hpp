#pragma once
#ifndef _TILEY_IMPL_HPP
#define _TILEY_IMPL_HPP


#include <vector>
#include <string>
#include <cassert>
#include "../error.hpp"
#include "../vector.hpp"
#include "../field2d.hpp"
#include "char_cast.hpp"

namespace tiley {
namespace impl {

template<typename BackImpl, typename CharCaster>
class TileyImpl
{
	static BackImpl* impl;

	class CheckObj
	{
	public:
		const CheckObj& operator %(TNERROR err) const
		{
			if(err > 0)
				throw TileyException();
			return  *this;
		}
	};
	static CheckObj chk() { return CheckObj(); }

public:
	static inline void DestroyObject(TNOBJ obj)
	{
		chk() %
			impl->destroy(obj);
	}

	static inline size_t FetchService(size_t timeout)
	{
		chk() %
			impl->fetch_service(&timeout);

		return timeout;
	}

	static inline void SetServiceThreadCount(size_t count)
	{
		chk() %
			impl->set_service_thread_count(count);
	}

	static inline TNSERVER CreateServer(const TNSVRCONFIG& init)
	{
		TNSERVER srv;

		chk() %
			impl->create_server(&srv, &init);

		return srv;
	}

	static inline TNACCEPTOR AddListenAcceptor(TNSERVER server, unsigned short port, unsigned int maxc)
	{
		TNACCEPTOR acc;

		chk() %
			impl->add_listen_acceptor(&acc, server, port, maxc);

		return acc;
	}

	static inline TNACCEPTOR AddLocalAcceptor(TNSERVER server)
	{
		TNACCEPTOR acc;

		chk() %
			impl->add_local_acceptor(&acc, server);

		return acc;
	}

	static inline void AddDetachedLocalAcceptor(TNSERVER server)
	{
		chk() %
			impl->add_local_acceptor(nullptr, server);
	}

	static inline std::vector<TNEVENT> FetchEvents(TNSERVER , size_t* timeout = nullptr)
	{
		std::vector<TNEVENT> evts(32);
		size_t fetched;

		chk() %
			tilenet_fetch_events(server, evts.data(), evts.size(), &fetched, nullptr);

		return evts;
	}

	static inline size_t Exit(size_t timeout = 0)
	{
		chk() %
			impl->exit(&timeout);

		return timeout;
	}

	template<typename Ch>
	static inline void KickParticipant(TNPARTICIPANT participant, const std::basic_string<Ch>& reason)
	{
		CharCaster caster;

		chk() %
			impl->kick(participant, caster.c_wstr(reason));
	}

	static inline void AttachLayer(TNPARTICIPANT participant, TNLAYER layer)
	{
		chk() %
			impl->kick(participant, layer);
	}

	template<typename Ch>
	static inline TNLAYER CreateFrame(const std::basic_string<Ch>& aspectName, TNFLAG flags)
	{
		CharCaster caster;
		TNLAYER frame;


		chk() %
			impl->create_frame(&frame, caster.c_wstr(aspectName), flags);

		return frame;
	}

	static inline void UpdateFrame(TNLAYER frame, const std::vector<TNLAYER>& layer_list, const std::vector<TNVIEW*>& view_list)
	{
		assert(layer_list.size() == view_list.size());
		chk() %
			impl->update_frame(frame, layer_list.data(), view_list.data(), layer_list.size());

	}

	template<typename Ch>
	static inline TNLAYER CreateTilelayer(Rect size, Ratio ratio, const std::basic_string<Ch>& aspect, TNFLAG flags)
	{
		CharCaster caster;
		TNLAYER layer;

		chk() %
			impl->create_tilelayer(size.w, size.h, ratio.x, ratio.y, caster.c_wstr(aspect), flags);

		return layer;
	}

	static inline void UpdateTilelayer(TNLAYER layer, const Field2D<TNTILE>& tiles, const Field2D<TNBOOL>& toupdate)
	{
		chk() %
			impl->update_tilelayer(layer, tiles.storage().data(), toupdate.storage().data());
	}

	template<typename Ch>
	static inline TNID Stdtile(const std::basic_string<Ch>& name)
	{
		CharCaster caster;
		TNID id = 0;

		chk() %
			impl->stdtile(caster.c_wstr(name), &id);

		return id;
	}

	template<typename Ch>
	static inline TNID KeynameToCode(const std::basic_string<Ch>& name)
	{
		TNKEYCODE keycode = 0;
		CharCaster caster;

		chk() %
			impl->keycode(caster.c_wstr(name), &keycode);

		return keycode;
	}

	template<typename Ch>
	static inline std::basic_string<Ch> KeycodeToName(TNKEYCODE code)
	{
		const wchar_t* keyname = nullptr;
		CharCaster caster;

		chk() %
			impl->keyname(code, &keyname);

		return caster.str<Ch>(keyname);
	}
};

template<typename BackImpl, typename CharCaster>
BackImpl* TileyImpl<BackImpl, CharCaster>::impl = nullptr;





}}


#endif