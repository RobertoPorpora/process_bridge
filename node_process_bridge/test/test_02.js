import { ChildProcessBridge } from '../ProcessBridge.js'

const args = process.argv.slice(2);
const child_command = args[0];
const command_args = args.slice(1);

let child = new ChildProcessBridge();
child.spawn(child_command, command_args);

let stdoutbuffer = [];
let stderrbuffer = [];

child.on('receive', (data) => {
    stdoutbuffer.push(data);
    terminator();
});

child.on('receive_err', (data) => {
    stderrbuffer.push(data);
    terminator();
});

function terminator() {
    if (stdoutbuffer.length > 2 && stderrbuffer.length > 2)
        child.despawn()
}

child.on('close', (code) => {
    if (stdoutbuffer[0] !== 'c1 p1 p2 p3')
        test_end(1)
    if (stderrbuffer[0] !== 'c1 p1 p2 p3')
        test_end(2)
    if (stdoutbuffer[1] !== 'c2')
        test_end(3)
    if (stderrbuffer[1] !== 'c2')
        test_end(4)
    if (stdoutbuffer[2] !== 'c3')
        test_end(5)
    if (stderrbuffer[2] !== 'c3')
        test_end(6)
    if (code !== null)
        test_end(7)
    test_end(0)
});

child.send('p1')
child.send('p2')
child.send('p3')

function test_end(number) {
    if (number !== 0) {
        console.log(`Test failed, code: ${number}`)
    } else {
        console.log(`Test passed, code: ${number}`)
    }
    process.exit(number)
}