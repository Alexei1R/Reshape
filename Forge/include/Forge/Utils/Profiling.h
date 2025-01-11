// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef PROFILING_H
#define PROFILING_H

#ifdef TRACY_BUILD_AND_LINKED
#    define TRACY_CALLSTACK 5
#    define TRACY_ENABLE
#    include <Tracy.hpp>

#    define PROFILE_FUNCTION() ZoneScoped;
#    define PROFILE_SCOPE(name) ZoneScopedN(name);
#    define PROFILE_EVENT(name) TracyEvent(name);
#else
#    define PROFILE_SCOPE(name)
#    define PROFILE_FUNCTION()
#    define PROFILE_EVENT(name)
#endif

#endif // PROFILING_H
