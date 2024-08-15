import { ChildProcessBridge } from '../ProcessBridge.js'

const args = process.argv.slice(2);
const child_command = args[0];
const command_args = args.slice(1);

console.log(`PARENT: Spawning ${child_command} ${command_args}`)
let child = new ChildProcessBridge();
child.spawn(child_command, command_args);

child.on('receive', (data) => {
    console.log(`PARENT: stdout: ${data}`);
});

child.on('receive_err', (data) => {
    console.log(`PARENT: stderr: ${data}`);
});

child.on('close', (code) => {
    console.log(`PARENT: Child process exited with code ${code}`);
});

setTimeout(() => {
    child.send(`Hello from parent process!`);
    console.log('PARENT: Wrote to child process.');
}, 1000);

setTimeout(() => {
    child.despawn();
    console.log('PARENT: Sent termination signal to child process');
}, 2000);
