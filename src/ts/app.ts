import { TEST } from './helper'
import { sendMessageToCPP } from './messages'
console.warn(TEST)

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

async function loadAssets() {
  // Load assets
  const aerowalkPointer = await loadFile("./data/aerowalk.bsp")
  _CPP_setCurrentMap(aerowalkPointer)

  const catImage = await loadImage("./data/poptart.jpg")
  if (!_CPP_createTexture(0, catImage.pointer, catImage.width, catImage.height)) {
    console.error('failed to load pop tart')
    return
  }

  const vertShader = await loadFile("./shader/test.vert")
  const fragShader = await loadFile("./shader/test.frag")
  if (!_CPP_createShaderProgram(0, vertShader, fragShader)) {
    console.error('failed to create shader program')
    return
  }

  _CPP_destroyBuffer(catImage.pointer)
  _CPP_destroyBuffer(vertShader)
  _CPP_destroyBuffer(fragShader)

  Module.sendMessageToCPP(JSON.stringify({
    type: "TestMessage",
    text: "This is a message from web"
  }))

  _CPP_start()
}

async function start() {
  loadAssets();
}

(function () {
  // Setup bindings
  window.MessageHandler = {
    handleMessageFromCPP: (json: string) => {
      const val = JSON.parse(json)
      console.warn(json)
      switch (val.type) {
        case 'CPPLoaded': {
          start()
          break
        }
        case 'TestMessage': {
          console.warn('received CPP => TS', JSON.parse(json))
          break
        }
      }
    }
  }
})();

