interface TestMessage {
  type: 'TestMessage'
  text: string;
}
interface CPPLoaded {
  type: 'CPPLoaded'
}
type Message = { type: 'Unknown' }  | TestMessage  | CPPLoaded 
function parseMessage(json: string): Message {
  const val = JSON.parse(json)
  switch (val.type) {
    case 'TestMessage': return val as TestMessage
    case 'CPPLoaded': return val as CPPLoaded
  }
  return { type: 'Unknown' }
}