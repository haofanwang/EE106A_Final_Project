/*
 * This file is part of ALVAR, A Library for Virtual and Augmented Reality.
 *
 * Copyright 2007-2012 VTT Technical Research Centre of Finland
 *
 * Contact: VTT Augmented Reality Team <alvar.info@vtt.fi>
 *          <http://www.vtt.fi/multimedia/alvar.html>
 *
 * ALVAR is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALVAR; if not, see
 * <http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html>.
 */

#include "Plugin_private.h"

#include "AlvarException.h"

#include <windows.h>
#include <sstream>

namespace alvar {

class PluginPrivateData
{
public:
    PluginPrivateData()
        : mHandle(NULL)
    {
    }
    
    HINSTANCE mHandle;
};

PluginPrivate::PluginPrivate()
    : d(new PluginPrivateData())
{
}

PluginPrivate::~PluginPrivate()
{
    delete d;
}

void PluginPrivate::load(const std::string filename)
{
    d->mHandle = LoadLibrary(filename.data());
    if (!d->mHandle) {
        std::stringstream message;
        message << "could not load " << filename
                << ", error code " << GetLastError();
        throw AlvarException(message.str().data());
    }
}

void PluginPrivate::unload()
{
    FreeLibrary(d->mHandle);
}

void *PluginPrivate::resolve(const char *symbol)
{
    void *address = (void *)GetProcAddress(d->mHandle, symbol);
    if (!address) {
        std::stringstream message;
        message << "could not resolve " << symbol;
        throw AlvarException(message.str().data());
    }
    return address;
}

} // namespace alvar
