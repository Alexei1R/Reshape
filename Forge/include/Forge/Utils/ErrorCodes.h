// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.E
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef ERRORCODES_H
#define ERRORCODES_H

#include "Forge/Utils/Log.h"
#include <cstdint>

namespace forge {

enum class ErrorCategory {
    None,      // No error
    System,    // System-level errors
    Render,    // Graphics and rendering
    Audio,     // Audio system
    Physics,   // Physics engine
    Resource,  // Resource management
    Scene,     // Scene graph and ECS
    Input,     // Input handling
    Network,   // Networking
    Script,    // Scripting engine
    Animation, // Animation system
    UI,        // User interface
    Debug,     // Debug and development
    Critical   // Critical system errors
};

enum class ErrorCode {
    // Success (0)
    Success = 0,

    // Parsing errors (10-50)

    // System errors (1000-1999)
    SystemBase = 1000,
    FileNotFound,         // 1001
    FileAccessDenied,     // 1002
    InvalidFilePath,      // 1003
    InsufficientMemory,   // 1004
    InvalidConfiguration, // 1005
    SystemInitFailed,     // 1006
    PermissionDenied,     // 1007
    InvalidOperation,     // 1008
    TimeoutError,         // 1009
    InvalidArgument,      // 1010

    // Render errors (2000-2999)
    RenderBase = 2000,
    ShaderCompilationFailed, // 2001
    TextureLoadFailed,       // 2002
    InvalidMesh,             // 2003
    SwapchainError,          // 2004
    FramebufferError,        // 2005
    InvalidRenderTarget,     // 2006
    PipelineCreationFailed,  // 2007
    InvalidShaderModule,     // 2008
    RenderPassError,         // 2009
    GraphicsDriverError,     // 2010

    // Audio errors (3000-3999)
    AudioBase = 3000,
    AudioDeviceNotFound, // 3001
    AudioFileLoadFailed, // 3002
    InvalidAudioFormat,  // 3003
    AudioPlaybackError,  // 3004
    AudioMixerError,     // 3005
    InvalidAudioState,   // 3006
    AudioStreamError,    // 3007
    AudioEncodingError,  // 3008

    // Physics errors (4000-4999)
    PhysicsBase = 4000,
    CollisionError,         // 4001
    InvalidRigidBody,       // 4002
    PhysicsSimulationError, // 4003
    InvalidCollider,        // 4004
    ConstraintError,        // 4005
    PhysicsSolverError,     // 4006

    // Resource errors (5000-5999)
    ResourceBase = 5000,
    ResourceLoadFailed,       // 5001
    ResourceNotFound,         // 5002
    InvalidResourceType,      // 5003
    ResourceCreationFailed,   // 5004
    ResourceAllocationFailed, // 5005
    DuplicateResource,        // 5006
    ResourceBusy,             // 5007

    // Scene errors (6000-6999)
    SceneBase = 6000,
    EntityCreationFailed, // 6001
    InvalidEntity,        // 6002
    ComponentError,       // 6003
    SceneLoadError,       // 6004
    InvalidSceneState,    // 6005
    SceneGraphError,      // 6006

    // Input errors (7000-7999)
    InputBase = 7000,
    InputDeviceNotFound, // 7001
    InputConfigError,    // 7002
    InvalidInputState,   // 7003
    InputBufferOverflow, // 7004
    InputMappingError,   // 7005

    // Network errors (8000-8999)
    NetworkBase = 8000,
    ConnectionFailed,    // 8001
    NetworkTimeout,      // 8002
    InvalidPacket,       // 8003
    NetworkDisconnected, // 8004
    ProtocolError,       // 8005
    InvalidNetworkState, // 8006

    // Script errors (9000-9999)
    ScriptBase = 9000,
    ScriptLoadError,      // 9001
    ScriptExecutionError, // 9002
    InvalidScriptState,   // 9003
    ScriptCompileError,   // 9004
    ScriptRuntimeError,   // 9005

    // Animation errors (10000-10999)
    AnimationBase = 10000,
    InvalidAnimation,      // 10001
    AnimationLoadError,    // 10002
    InvalidSkeleton,       // 10003
    AnimationBlendError,   // 10004
    InvalidAnimationState, // 10005

    // UI errors (11000-11999)
    UIBase = 11000,
    UILayoutError,    // 11001
    InvalidUIElement, // 11002
    UIRenderError,    // 11003
    UIEventError,     // 11004
    UIStateError,     // 11005

    // Debug errors (12000-12999)
    DebugBase = 12000,
    DebugAssertionFailed, // 12001
    InvalidDebugState,    // 12002
    DebugCommandFailed,   // 12003
    ProfilerError,        // 12004

    // Critical errors (13000-13999)
    CriticalBase = 13000,
    EngineInitFailed,    // 13001
    FatalError,          // 13002
    SystemShutdownError, // 13003
    UnrecoverableError   // 13004
};

class ErrorResult {
public:
    constexpr ErrorResult()
        : m_Code(0) {}
    constexpr ErrorResult(ErrorCode code)
        : m_Code(static_cast<uint32_t>(code)) {}

    constexpr bool IsSuccess() const {
        return m_Code == 0;
    }

    constexpr bool IsError() const {
        return m_Code != 0;
    }

    constexpr ErrorCode Code() const {
        return static_cast<ErrorCode>(m_Code);
    }

    constexpr operator bool() const {
        return IsSuccess();
    }

    constexpr ErrorCategory GetErrorCategory() {

        uint32_t numericCode = static_cast<uint32_t>(m_Code);
        if (numericCode == 0) {
            return ErrorCategory::None;
        }
        // Check the error code ranges
        if (numericCode >= 1000 && numericCode < 2000) {
            return ErrorCategory::System;
        }
        if (numericCode >= 2000 && numericCode < 3000) {
            return ErrorCategory::Render;
        }
        if (numericCode >= 3000 && numericCode < 4000) {
            return ErrorCategory::Audio;
        }
        if (numericCode >= 4000 && numericCode < 5000) {
            return ErrorCategory::Physics;
        }
        if (numericCode >= 5000 && numericCode < 6000) {
            return ErrorCategory::Resource;
        }
        if (numericCode >= 6000 && numericCode < 7000) {
            return ErrorCategory::Scene;
        }
        if (numericCode >= 7000 && numericCode < 8000) {
            return ErrorCategory::Input;
        }
        if (numericCode >= 8000 && numericCode < 9000) {
            return ErrorCategory::Network;
        }
        if (numericCode >= 9000 && numericCode < 10000) {
            return ErrorCategory::Script;
        }
        if (numericCode >= 10000 && numericCode < 11000) {
            return ErrorCategory::Animation;
        }
        if (numericCode >= 11000 && numericCode < 12000) {
            return ErrorCategory::UI;
        }
        if (numericCode >= 12000 && numericCode < 13000) {
            return ErrorCategory::Debug;
        }
        if (numericCode >= 13000 && numericCode < 14000) {
            return ErrorCategory::Critical;
        }

        return ErrorCategory::None;
    }

private:
    uint32_t m_Code;
};

} // namespace forge

#endif
