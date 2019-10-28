let project = new Project('New Project');

project.addFile('Sources/**');
project.setDebugDir('Deployment');

resolve(project);
