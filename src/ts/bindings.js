export var ResourceType;
(function (ResourceType) {
    ResourceType[ResourceType["BSP_FILE"] = 0] = "BSP_FILE";
    ResourceType[ResourceType["IMAGE_FILE"] = 1] = "IMAGE_FILE";
    ResourceType[ResourceType["UNKNOWN"] = 2] = "UNKNOWN";
})(ResourceType || (ResourceType = {}));
;
export function parseMessage(json) {
    const val = JSON.parse(json);
    switch (val.type) {
        case 'TestMessage': return val;
        case 'CPPLoaded': return val;
        case 'LoadResource': return val;
        case 'LoadShaders': return val;
        case 'LoadedShaders': return val;
        case 'LoadedTexture': return val;
        case 'MissingTexture': return val;
        case 'LoadedBSP': return val;
        case 'LoadedTextureOptions': return val;
    }
    return { type: 'Unknown' };
}
//# sourceMappingURL=bindings.js.map