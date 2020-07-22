import { parseMessage, LoadResource, Message, ResourceType, LoadShaders } from './bindings'
import { isEmpty } from './helper'

async function loadFile(src: string) {
  const blob = await fetch(src).then(resp => resp.blob())
  const pointer = _CPP_createBuffer(blob.size)
  const buffer = await blob.arrayBuffer()

  const data = new Uint8ClampedArray(buffer)
  Module.HEAP8.set(data, pointer)

  return {
    pointer,
    length: blob.size
  }
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

type TextureManifest = { textures: {[key: string]: boolean}, shaders: {[key: string]: Array<string[]>} }
let _textureManifest: TextureManifest = { textures: {}, shaders: {} }
async function getTextureManifest() {
  if (!isEmpty(_textureManifest.textures)) {
    return _textureManifest
  }
  _textureManifest = await fetch('./data/textures_manifest.json')
    .then(resp => resp.json()) as TextureManifest

  return _textureManifest
}

function findTextureInManifest(url: string, manifest: TextureManifest) {
  url = url.replace(/^(\.\/)/,"")
  if (url in manifest.textures) {
    return url
  }
  
  if ((url + '.jpg') in manifest.textures) {
    return url + '.jpg'
  }
  
  if ((url + '.png') in manifest.textures) {
    return url + '.png'
  }

  return undefined
}

function findTextureShader(url: string, manifest: TextureManifest) {
  url = url.replace(/^.*data\//,"")
  url = url.replace(/\.[A-z]+$/,"")

  if (url in manifest.shaders) {
    return manifest.shaders[url]
  }
  
  if ((url + '.jpg') in manifest.shaders) {
    return manifest.shaders[(url + '.jpg')]
  }
  
  if ((url + '.png') in manifest.shaders) {
    return manifest.shaders[(url + '.png')]
  }

  return undefined
}

async function loadResource(message: LoadResource) {
  switch (message.resourceType) {
    case ResourceType.BSP_FILE: {
      const { pointer } = await loadFile(message.url)
      sendMessageFromWeb({
        type: 'LoadedBSP',
        resourceID: message.resourceID,
        pointer: pointer
      })

      break
    }
    case ResourceType.IMAGE_FILE: {
      const textureManifest = await getTextureManifest()
      const textureUrl = findTextureInManifest(message.url, textureManifest)
      if (textureUrl) {
        const shaderForTexture = findTextureShader(textureUrl, textureManifest)
        if (shaderForTexture) {
          console.warn('shader for', textureUrl, '=>', shaderForTexture)
        }

        const image = await loadImage(textureUrl)
        sendMessageFromWeb({
          type: 'LoadedTexture',
          resourceID: message.resourceID,
          pointer: image.pointer,
          width: image.width,
          height: image.height
        })
      } else {
        sendMessageFromWeb({
          type: 'MissingTexture',
          resourceID: message.resourceID,
        })
      }
      break
    }    
  }
}

async function loadShaders(message: LoadShaders) {
  const vert = await loadFile(message.vertUrl)
  const frag = await loadFile(message.fragUrl)
  sendMessageFromWeb({
    type: 'LoadedShaders',
    resourceID: message.resourceID,
    vertPointer: vert.pointer,
    fragPointer: frag.pointer,
    vertLength: vert.length,
    fragLength: frag.length
  })
}

function sendMessageFromWeb(message: Message) {
  Module.sendMessageToCPP(JSON.stringify(message))
}

// Setup bindings
window.MessageHandler = {
  handleMessageFromCPP: (json: string) => {
    const message = parseMessage(json)
    console.log('CPP => TS w/', message)
    switch (message.type) {
      case 'CPPLoaded': {
        start()
        break
      }
      case 'LoadResource': {
        loadResource(message)
        break
      }
      case 'LoadShaders': {
        loadShaders(message)
        break
      }
    }
  }
}

async function start() {
  sendMessageFromWeb({
    type: "TestMessage",
    text: "start() called in TS"
  })
}

