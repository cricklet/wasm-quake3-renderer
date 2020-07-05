declare var Module: {
  sendMessageToCPP: (s: string) => void
} & EmscriptenModule

interface MessageHandler {
  handleMessageFromCPP: (s: string) => void
}

interface Window {
  MessageHandler: MessageHandler
}

declare function _CPP_createBuffer(bytes: number): number
declare function _CPP_destroyBuffer(pointer: number): void
