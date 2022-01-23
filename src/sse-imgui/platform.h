/**
 * @file platform.h
 * @brief Detect the current operating environment
 * @internal
 *
 * This file is part of SSE Hooks project (aka SSE-ImGui).
 *
 *   SSE-ImGui is free software: you can redistribute it and/or modify it
 *   under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   SSE-ImGui is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with SSE-ImGui. If not, see <http://www.gnu.org/licenses/>.
 *
 * @endinternal
 *
 * @ingroup Public API
 *
 * @details
 * This file contains conditional macro definitions determining compile time
 * attributes according to the operating environment. As of now it detects
 * the intended operating system where the exectuable will run under, which
 * compiler is used for the build and API build helpers.
 */

#ifndef SSEIMGUI_PLATFORM_H
#define SSEIMGUI_PLATFORM_H

/*----------------------------------------------------------------------------*/
/* Select operating system: */

#undef SSEIMGUI_WINDOWS
#undef SSEIMGUI_POSIX

#if defined(_WIN32) \
    || defined(_WIN64) \
    || defined(__WIN32__) \
    || defined(__WINDOWS__) \
    || defined(__MINGW32__) \
    || defined(__MINGW64__)

/** Defined when targeting Microsoft Windows operating system */
#define SSEIMGUI_WINDOWS

#else

/** Defined when NOT targeting Windows but POSIX compatible system */
#define SSEIMGUI_POSIX

#endif

/*----------------------------------------------------------------------------*/
/* Select compiler: */

#undef SSEIMGUI_GNUC
#undef SSEIMGUI_MSVC
#undef SSEIMGUI_MINGW

#if defined(__GNUC__)

/** Any GNU GCC C++ compiler */
#define SSEIMGUI_GNUC \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if defined(__MINGW32__) || defined(__MINGW64__)
/** GNU GCC as cross compiler or native under Windows. */
#define SSEIMGUI_MINGW SSEIMGUI_GNUC
#endif

#elif defined(_MSC_VER) /* Last as other vendors also define this. */

/** Any Microsoft Visual Studio C++ compiler. */
#define SSEIMGUI_MSVC (_MSC_VER)

#endif

/*----------------------------------------------------------------------------*/
/* Select the C calling convention: */

#undef SSEIMGUI_CCONV

#if defined(SSEIMGUI_WINDOWS) && !defined(SSEIMGUI_MINGW)
#if defined(SSEIMGUI_GNUC)
/** GCC on Windows understands stdcall */
#define SSEIMGUI_CCONV __attribute__((stdcall))

#elif defined(SSEIMGUI_MSVC)
/** Visual C++ on Windows uses stdcall */
#define SSEIMGUI_CCONV __stdcall
#endif

#elif defined(SSEIMGUI_POSIX) || defined(SSEIMGUI_MINGW)
/** Linux/Unix/Cross and etc. use only one type of convention */
#define SSEIMGUI_CCONV

#endif

/*----------------------------------------------------------------------------*/
/* Select the shared library interface */

#undef SSEIMGUI_API

#if defined(SSEIMGUI_WINDOWS)

/* In practice this is defined as paramater to the build. */
#if defined(SSEIMGUI_BUILD_API)
/** The current build exposes DLL functions */
#define SSEIMGUI_API __declspec(dllexport)

#else
/** The current build imports, previously exported DLL functions */
#define SSEIMGUI_API __declspec(dllimport)
#endif

#elif defined(SSEIMGUI_POSIX)
/** The current build does not use any specific storage information */
#define SSEIMGUI_API

#endif

/*----------------------------------------------------------------------------*/

#endif

