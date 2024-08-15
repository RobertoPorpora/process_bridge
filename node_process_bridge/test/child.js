import { ParentProcessBridge } from '../ProcessBridge.js'

let buffer = []
let parent = new ParentProcessBridge()

parent.on('end', () => {
    process.exit(0);
});

parent.on('receive', (data) => {
    buffer.push(data);
    if (buffer.length > 2) {
        second_stage();
    }
})

function second_stage() {
    let msgs = [
        `c1 ${buffer.join(' ')}`,
        'c2',
        'c3',
    ]
    for (const msg of msgs) {
        parent.send(msg)
        parent.send_err(msg)
    }
    setTimeout(() => {
        process.exit(12);
    }, 1000);
}    
