let project = new Project('DearImGui');

project.addDefine('IMGUI_USER_CONFIG="imguiconfig.h"');
project.addIncludeDir('include');

project.addIncludeDir('imgui');
project.addFiles('Sources/**', 'include/**', 'Shaders/**');
project.setDebugDir('Deployment');

resolve(project);
