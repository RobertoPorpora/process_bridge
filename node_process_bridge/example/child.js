import { ParentProcessBridge } from '../ProcessBridge.js'

let parent = new ParentProcessBridge()
parent.on('receive', (data) => {
    parent.send('CHILD: Received data: ' + data)
})

setTimeout(() => {
    parent.send('CHILD: Message sent to stdout after a short delay.');
}, 300);

setTimeout(() => {
    parent.send_err('CHILD: Error message sent to stderr after a longer delay.');
}, 700);

parent.on('end', () => {
    process.exit(0);
});

setTimeout(() => {
    parent.send('CHILD: Exiting with code 12.');
    process.exit(12);
}, 1200);
