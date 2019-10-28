let project = new Project('DearImGui');

project.addIncludeDir('imgui');
project.addFile('Sources/**');
project.setDebugDir('Deployment');

resolve(project);
