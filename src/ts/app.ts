import { parseMessage, LoadResource, Message, ResourceType } from './bindings'

async function loadFile(src: string) {
  const blob = await fetch(src).then(resp => resp.blob())
  const pointer = _CPP_createBuffer(blob.size)
  const buffer = await blob.arrayBuffer()

  const data = new Uint8ClampedArray(buffer)
  Module.HEAP8.set(data, pointer)

  return pointer
}

const imageCanvas = document.createElement('canvas');
async function loadImage(src: string) {
  const imgBlob = await fetch(src).then(resp => resp.blob());
  const imageBitmap = await createImageBitmap(imgBlob);

  // Make canvas same size as image
  imageCanvas.width = imageBitmap.width;
  imageCanvas.height = imageBitmap.height;

  // Draw image onto imageCanvas
  const ctx = imageCanvas.getContext('2d') as CanvasRenderingContext2D;
  ctx.drawImage(imageBitmap, 0, 0);

  // Get image data
  const image = ctx.getImageData(0, 0, imageBitmap.width, imageBitmap.height);

  const pointer = _CPP_createBuffer(image.width * image.height * 4)
  Module.HEAP8.set(image.data, pointer)

  return { pointer, width: image.width, height: image.height }
}

async function start() {
  sendMessageFromWeb({
    type: "TestMessage",
    text: "start() called in TS"
  })
}

async function loadResource(message: LoadResource) {
  switch (message.resourceType) {
    case ResourceType.BSP_FILE: {
      const pointer = await loadFile(message.url)
      sendMessageFromWeb({
        type: 'LoadedBSP',
        resourceID: message.resourceID,
        pointer: pointer
      })

      break
    }
    case ResourceType.IMAGE_FILE: {
      const image = await loadImage(message.url)
      sendMessageFromWeb({
        type: 'LoadedImage',
        resourceID: message.resourceID,
        pointer: image.pointer,
        width: image.width,
        height: image.height
      })
      break
    }    
  }
}

function sendMessageFromWeb(message: Message) {
  Module.sendMessageToCPP(JSON.stringify(message))
}

// Setup bindings
window.MessageHandler = {
  handleMessageFromCPP: (json: string) => {
    const message = parseMessage(json)
    switch (message.type) {
      case 'CPPLoaded': {
        start()
        break
      }
      case 'TestMessage': {
        console.warn('received CPP => TS', message)
        break
      }
      case 'LoadResource': {
        loadResource(message)
        break
      }
    }
  }
}

