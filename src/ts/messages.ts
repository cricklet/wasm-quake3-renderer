
interface TestMessage {
  type: "TestMessage"
  text: string
}

type Message = TestMessage

export const sendMessageToCPP = (message: Message) => {
  // const text: string = JSON.stringify(message)
  // _sendMessageToCPP(text)ss
}