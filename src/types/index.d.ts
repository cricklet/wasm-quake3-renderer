declare var Module: {
  sendMessageToCPP: (s: string) => void
  createBuffer: (bytes: number) => Promise<number>
} & EmscriptenModule

interface MessageHandler {
  handleMessageFromCPP: (s: string) => void
}

interface Window {
  MessageHandler: MessageHandler
  isOSX?: boolean
}

declare function _CPP_createBuffer(bytes: number): number
declare function _CPP_destroyBuffer(pointer: number): void
