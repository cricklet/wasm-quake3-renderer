var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
import { parseMessage, ResourceType } from './bindings';
import { isEmpty } from './helper';
function loadFile(src) {
    return __awaiter(this, void 0, void 0, function* () {
        const blob = yield fetch(src).then(resp => resp.blob());
        const pointer = _CPP_createBuffer(blob.size);
        const buffer = yield blob.arrayBuffer();
        const data = new Uint8ClampedArray(buffer);
        Module.HEAP8.set(data, pointer);
        return {
            pointer,
            length: blob.size
        };
    });
}
const imageCanvas = document.createElement('canvas');
function loadImage(src) {
    return __awaiter(this, void 0, void 0, function* () {
        const imgBlob = yield fetch(src).then(resp => resp.blob());
        const imageBitmap = yield createImageBitmap(imgBlob);
        // Make canvas same size as image
        imageCanvas.width = imageBitmap.width;
        imageCanvas.height = imageBitmap.height;
        // Draw image onto imageCanvas
        const ctx = imageCanvas.getContext('2d');
        ctx.drawImage(imageBitmap, 0, 0);
        // Get image data
        const image = ctx.getImageData(0, 0, imageBitmap.width, imageBitmap.height);
        const pointer = _CPP_createBuffer(image.width * image.height * 4);
        Module.HEAP8.set(image.data, pointer);
        return { pointer, width: image.width, height: image.height };
    });
}
let _textureManifest = { textures: {}, shaders: {} };
function getTextureManifest() {
    return __awaiter(this, void 0, void 0, function* () {
        if (!isEmpty(_textureManifest.textures)) {
            return _textureManifest;
        }
        _textureManifest = (yield fetch('./data/textures_manifest.json')
            .then(resp => resp.json()));
        return _textureManifest;
    });
}
function findTextureInManifest(url, manifest) {
    url = url.replace(/^(\.\/)/, "");
    if (url in manifest.textures) {
        return url;
    }
    if ((url + '.jpg') in manifest.textures) {
        return url + '.jpg';
    }
    if ((url + '.png') in manifest.textures) {
        return url + '.png';
    }
    return undefined;
}
function findTextureOptions(url, manifest) {
    url = url.replace(/^.*data\//, "");
    url = url.replace(/\.[A-z]+$/, "");
    let rawShader;
    if (url in manifest.shaders) {
        rawShader = manifest.shaders[url];
    }
    if ((url + '.jpg') in manifest.shaders) {
        rawShader = manifest.shaders[(url + '.jpg')];
    }
    if ((url + '.png') in manifest.shaders) {
        rawShader = manifest.shaders[(url + '.png')];
    }
    if (!rawShader) {
        return undefined;
    }
    const result = {};
    for (const line of rawShader) {
        if (line[0] === 'qer_trans') {
            result['qer_trans'] = parseFloat(line[1]);
        }
    }
    return result;
}
function loadResource(message) {
    return __awaiter(this, void 0, void 0, function* () {
        switch (message.resourceType) {
            case ResourceType.BSP_FILE: {
                const { pointer } = yield loadFile(message.url);
                sendMessageFromWeb({
                    type: 'LoadedBSP',
                    resourceID: message.resourceID,
                    pointer: pointer
                });
                break;
            }
            case ResourceType.IMAGE_FILE: {
                const textureManifest = yield getTextureManifest();
                const textureUrl = findTextureInManifest(message.url, textureManifest);
                if (textureUrl) {
                    const image = yield loadImage(textureUrl);
                    sendMessageFromWeb({
                        type: 'LoadedTexture',
                        resourceID: message.resourceID,
                        pointer: image.pointer,
                        width: image.width,
                        height: image.height
                    });
                    const shaderForTexture = findTextureOptions(textureUrl, textureManifest);
                    if (shaderForTexture) {
                        console.warn('shader for', textureUrl, '=>', shaderForTexture);
                        sendMessageFromWeb({
                            type: 'LoadedTextureOptions',
                            resourceID: message.resourceID,
                            transparency: shaderForTexture.qer_trans === undefined ? 1 : shaderForTexture.qer_trans
                        });
                    }
                }
                else {
                    sendMessageFromWeb({
                        type: 'MissingTexture',
                        resourceID: message.resourceID,
                    });
                }
                break;
            }
        }
    });
}
function loadShaders(message) {
    return __awaiter(this, void 0, void 0, function* () {
        const vert = yield loadFile(message.vertUrl);
        const frag = yield loadFile(message.fragUrl);
        sendMessageFromWeb({
            type: 'LoadedShaders',
            resourceID: message.resourceID,
            vertPointer: vert.pointer,
            fragPointer: frag.pointer,
            vertLength: vert.length,
            fragLength: frag.length
        });
    });
}
function sendMessageFromWeb(message) {
    Module.sendMessageToCPP(JSON.stringify(message));
}
// Setup bindings
window.MessageHandler = {
    handleMessageFromCPP: (json) => {
        const message = parseMessage(json);
        console.log('CPP => TS w/', message);
        switch (message.type) {
            case 'CPPLoaded': {
                start();
                break;
            }
            case 'LoadResource': {
                loadResource(message);
                break;
            }
            case 'LoadShaders': {
                loadShaders(message);
                break;
            }
        }
    }
};
function start() {
    return __awaiter(this, void 0, void 0, function* () {
        sendMessageFromWeb({
            type: "TestMessage",
            text: "start() called in TS"
        });
    });
}
//# sourceMappingURL=app.js.map