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

// Looks like: [
//   ["qer_editorimage", "textures/proto2/beam_blue.tga"],
//   ["surfaceparm", "trans"],
//   ["surfaceparm", "nomarks"],
//   ["surfaceparm", "nonsolid"],
//   ["surfaceparm", "nolightmap"],
//   ["qer_trans", ".6"],
//   ["cull", "none"],
//   [
//     ["map", "textures/proto2/beam_blue.tga"],
//     ["blendfunc", "add"]
//   ]
// ]
type RawTextureOptions = Array<any>

type TextureManifest = { textures: {[key: string]: boolean}, shaders: {[key: string]: RawTextureOptions} }
let _textureManifest: TextureManifest = { textures: {}, shaders: {} }
async function getTextureManifest() {
  if (!isEmpty(_textureManifest.textures)) {
    return _textureManifest
  }
  _textureManifest = await fetch('./data/textures_manifest.json')
    .then(resp => resp.json()) as TextureManifest

  return _textureManifest
}

function findTextureInManifest(url: string, manifest: TextureManifest): string | undefined {
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

type TextureOptions = {
  surfaceParamTrans?: boolean
}

function findTextureOptions(url: string, manifest: TextureManifest) : TextureOptions | undefined {
  url = url.replace(/^.*data\//,"")
  url = url.replace(/\.[A-z]+$/,"")

  let rawShader
  if (url in manifest.shaders) {
    rawShader = manifest.shaders[url]
  }
  
  if ((url + '.jpg') in manifest.shaders) {
    rawShader = manifest.shaders[(url + '.jpg')]
  }
  
  if ((url + '.png') in manifest.shaders) {
    rawShader = manifest.shaders[(url + '.png')]
  }

  if (!rawShader) {
    return undefined
  }

  const result: TextureOptions = {}
  for (const line of rawShader) {
    if (line[0] == 'surfaceparm' && line[1] == 'trans') {
      result['surfaceParamTrans'] = true
    }
  }

  return result;
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
        const image = await loadImage(textureUrl)
        sendMessageFromWeb({
          type: 'LoadedTexture',
          resourceID: message.resourceID,
          pointer: image.pointer,
          width: image.width,
          height: image.height
        })
        const shaderForTexture = findTextureOptions(textureUrl, textureManifest)
        if (shaderForTexture) {
          console.warn('shader for', textureUrl, '=>', shaderForTexture)
          sendMessageFromWeb({
            type: 'LoadedTextureOptions',
            resourceID: message.resourceID,
            surfaceParamTrans: shaderForTexture.surfaceParamTrans === true
          })
        }

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

// TODO only send this if we're in the XCode build??? Not sure yet.
sendMessageFromWeb({
  type: "TSLoaded"
})