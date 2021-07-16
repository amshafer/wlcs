/*
 * Copyright © 2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2 or 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by:
 *   Alexandros Frantzis <alexandros.frantzis@canonical.com>
 */

#include "helpers.h"
#include "shared_library.h"

#include <boost/throw_exception.hpp>
#include <system_error>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/syscall.h>

int wlcs::helpers::create_anonymous_file(size_t size)
{

    int fd = memfd_create("wlcs-unnamed", MFD_CLOEXEC);

    if (fd == -1)
    {
        BOOST_THROW_EXCEPTION(
            std::system_error(errno, std::system_category(), "Failed to open temporary file"));
    }

    if (ftruncate(fd, size) == -1)
    {
        close(fd);
        BOOST_THROW_EXCEPTION(
            std::system_error(errno, std::system_category(), "Failed to resize temporary file"));
    }

    return fd;
}

namespace
{
static int argc;
static char const** argv;

std::shared_ptr<WlcsServerIntegration const> entry_point;
}

void wlcs::helpers::set_command_line(int argc, char const** argv)
{
    ::argc = argc;
    ::argv = argv;
}

int wlcs::helpers::get_argc()
{
    return ::argc;
}

char const** wlcs::helpers::get_argv()
{
    return ::argv;
}

void wlcs::helpers::set_entry_point(std::shared_ptr<WlcsServerIntegration const> const& entry_point)
{
    ::entry_point = entry_point;
}

std::shared_ptr<WlcsServerIntegration const> wlcs::helpers::get_test_hooks()
{
    return ::entry_point;
}
