let project = new Project('DearImGui');

project.addDefine('IMGUI_USER_CONFIG="imguiconfig.h"');
project.addIncludeDir('include');

project.addIncludeDir('imgui');
project.addFiles(
	'imgui/imgui.cpp',
	'imgui/imgui.h',
	'imgui/imgui_draw.cpp',
	'imgui/imgui_widgets.cpp'
);

project.addFiles('Sources/**', 'include/**', 'Shaders/**');
project.setDebugDir('Deployment');

resolve(project);
