declare var Module: {
  sendMessageToCPP: (s: string) => void
  createBuffer: (bytes: number) => Promise<any>
} & EmscriptenModule

interface MessageHandler {
  handleMessageFromCPP: (s: string) => void
}

interface Window {
  MessageHandler: MessageHandler
  isOSX?: boolean
}
