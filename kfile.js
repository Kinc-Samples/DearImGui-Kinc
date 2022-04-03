const project = new Project('DearImGui');

await project.addProject('Kinc');

project.addDefine('IMGUI_USER_CONFIG="imguiconfig.h"');
project.addIncludeDir('include');

project.addIncludeDir('imgui');
project.addFiles(
	'imgui/imgui.cpp',
	'imgui/imgui.h',
	'imgui/imgui_draw.cpp',
	'imgui/imgui_tables.cpp',
	'imgui/imgui_widgets.cpp'
);

project.addFiles('Sources/**', 'include/**', 'Shaders/**');
project.setDebugDir('Deployment');

project.flatten();

resolve(project);
