/*
 * Copyright (C) 2003-2014 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _LIBUPNP_H_X_INCLUDED_
#define _LIBUPNP_H_X_INCLUDED_

#include "thread/Mutex.hxx"
#include "util/Error.hxx"

#include <string>
#include <map>

#include <upnp/upnp.h>
#include <upnp/upnptools.h>

/** Our link to libupnp. Initialize and keep the handle around */
class LibUPnP {
	// A Handler object records the data from registerHandler.
	class Handler {
	public:
		Handler(Upnp_FunPtr h, void *c)
			: handler(h), cookie(c) {}
		Upnp_FunPtr handler;
		void *cookie;
	};

	Error init_error;
	UpnpClient_Handle m_clh;
	Mutex m_mutex;
	std::map<Upnp_EventType, Handler> m_handlers;

	LibUPnP();

	LibUPnP(const LibUPnP &) = delete;
	LibUPnP &operator=(const LibUPnP &) = delete;

	static int o_callback(Upnp_EventType, void *, void *);

public:
	~LibUPnP();

	/** Retrieve the singleton LibUPnP object */
	static LibUPnP *getLibUPnP(Error &error);

	/** Set log file name and activate logging.
	 *
	 * @param fn file name to use. Use empty string to turn logging off
	 */
	bool setLogFileName(const std::string& fn);

	/** Set max library buffer size for reading content from servers.
	 * The default is 200k and should be ok */
	void setMaxContentLength(int bytes);

	/** Check state after initialization */
	bool ok() const
	{
		return !init_error.IsDefined();
	}

	/** Retrieve init error if state not ok */
	const Error &GetInitError() const {
		return init_error;
	}

	void registerHandler(Upnp_EventType et, Upnp_FunPtr handler, void *cookie);

	UpnpClient_Handle getclh()
	{
		return m_clh;
	}
};

#endif /* _LIBUPNP.H_X_INCLUDED_ */
