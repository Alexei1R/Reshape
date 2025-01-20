diff --git a/CMakeLists.txt b/CMakeLists.txt
index d4e99f8..745f8ef 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -111,6 +111,10 @@ message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID}")
 message(STATUS "Base Compiler Flags: ${CMAKE_CXX_FLAGS}")
 message(STATUS "Build-Specific Flags: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")
 
+
+# Root path
+set(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
+
 #-------------------------------------------------------------------------------
 # Targets / Subdirectories
 #-------------------------------------------------------------------------------
diff --git a/Forge/src/Renderer/Shader.cpp b/Forge/src/Renderer/Shader.cpp
index a76b855..4a4b9d3 100644
--- a/Forge/src/Renderer/Shader.cpp
+++ b/Forge/src/Renderer/Shader.cpp
@@ -87,7 +87,6 @@ std::unordered_map<ShaderType, std::vector<uint32_t>> GenerateAndCacheSPIRV(cons
                     spirv.resize(size / sizeof(uint32_t));
                     inFile.read(reinterpret_cast<char*>(spirv.data()), size);
                     spirvBinaries[type] = std::move(spirv);
-                    Log::Trace("Loaded cached SPIR-V shader: {}", shaderCachePath.string());
                     continue;
                 }
             } catch (const std::exception& e) {
diff --git a/Forge/src/Renderer/Shader/ShaderParser.cpp b/Forge/src/Renderer/Shader/ShaderParser.cpp
index cdbba7e..567800d 100644
--- a/Forge/src/Renderer/Shader/ShaderParser.cpp
+++ b/Forge/src/Renderer/Shader/ShaderParser.cpp
@@ -1,8 +1,8 @@
 // Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
 // Distributed under the MIT License (http://opensource.org/licenses/MIT)
 
-#include "Forge/Renderer/Shader.h"
 #include "Forge/Renderer/Shader/ShaderParser.h"
+#include "Forge/Renderer/Shader.h"
 #include "Forge/Utils/Common.h"
 #include "Forge/Utils/Log.h"
 #include <algorithm>
@@ -88,7 +88,6 @@ ErrorResult ShaderParser::ParseString(const std::string& shader) noexcept {
             auto it = m_ShaderTypeMap.find(typeName);
             if (it != m_ShaderTypeMap.end()) {
                 currentShaderType = it->second;
-                Log::Trace("Found shader type: {}", typeName);
             } else {
                 Log::Critical("Unknown shader type: {}", typeName);
                 return ErrorCode::InvalidArgument;
@@ -97,7 +96,6 @@ ErrorResult ShaderParser::ParseString(const std::string& shader) noexcept {
         // Check for the #name directive
         else if (line.rfind("#name", 0) == 0) { // If line starts with #name
             m_ShaderName = line.substr(6);      // Get the name after #name
-            Log::Trace("Found shader name: {}", m_ShaderName);
         } else {
             // MOTE: If the current line is not a type add it to the shader
             if (currentShaderType != ShaderType::Unknown) {
diff --git a/Forge/src/Utils/FileSystem.cpp b/Forge/src/Utils/FileSystem.cpp
index 3672d5b..b097595 100644
--- a/Forge/src/Utils/FileSystem.cpp
+++ b/Forge/src/Utils/FileSystem.cpp
@@ -22,6 +22,7 @@ std::string FileSystem::s_ApplicationName;
 bool FileSystem::s_Initialized = false;
 
 void FileSystem::Init(const std::string& applicationName) {
+    PROJECT_ROOT_DIR
     if (s_Initialized) {
         Log::Warn("FileSystem already initialized!");
         return;
diff --git a/Reshape/CMakeLists.txt b/Reshape/CMakeLists.txt
index dbc9826..ac5536a 100644
--- a/Reshape/CMakeLists.txt
+++ b/Reshape/CMakeLists.txt
@@ -1,4 +1,3 @@
-
 #-------------------------------------------------------------------------------
 #  RESHAPE CONFIGURATION
 #-------------------------------------------------------------------------------
@@ -25,6 +24,10 @@ target_link_libraries(${TARGET} PRIVATE
     Forge
 )
 
+target_compile_definitions(${TARGET} PRIVATE 
+    ROOT_PATH="${ROOT_PATH}"
+)
+
 
 
 
diff --git a/Reshape/src/Application.cpp b/Reshape/src/Application.cpp
index ff5e03a..9e279a5 100644
--- a/Reshape/src/Application.cpp
+++ b/Reshape/src/Application.cpp
@@ -48,7 +48,7 @@ Application::Application() {
         glBindVertexArray(0);
     }
 
-    m_Shader = forge::Shader::Create("/home/toor/Code/Reshape/shaders/main.glsl", forge::ShaderOrigin::File);
+    m_Shader = forge::Shader::Create("shaders/main.glsl", forge::ShaderOrigin::File);
     m_Shader->Bind();
 
     for (const auto& input : m_Shader->GetStageInputs()) {
diff --git a/Reshape/src/main.cpp b/Reshape/src/main.cpp
index 6f3bae0..a59b1c3 100644
--- a/Reshape/src/main.cpp
+++ b/Reshape/src/main.cpp
@@ -5,12 +5,20 @@
 
 #include "Application.h"
 #include "Forge/Utils/FileSystem.h"
+#include "Forge/Utils/Log.h"
 #include "Utils/Parsing.h"
 
 int main(int argc, char* argv[]) {
     forge::Log::Init("Reshape");
     forge::FileSystem::Init("Reshape");
 
+    std::filesystem::path rootPath = ROOT_PATH;
+    try {
+        std::filesystem::current_path(rootPath);
+    } catch (const std::filesystem::filesystem_error& ex) {
+        forge::Log::Critical("Error setting current path:  {0}", ex.what());
+    }
+
     {
         // TODO: Cleanup this code later
         bool apiSpecified;
