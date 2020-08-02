export enum ResourceType {
  BSP_FILE,
  IMAGE_FILE,
  UNKNOWN
};
export interface TestMessage {
  type: 'TestMessage'
  text: string;
}
export interface CPPLoaded {
  type: 'CPPLoaded'
}
export interface LoadResource {
  type: 'LoadResource'
  url: string;
  resourceType: ResourceType;
  resourceID: number;
}
export interface LoadShaders {
  type: 'LoadShaders'
  vertUrl: string;
  fragUrl: string;
  resourceID: number;
}
export interface LoadedShaders {
  type: 'LoadedShaders'
  resourceID: number;
  vertPointer: number;
  fragPointer: number;
  vertLength: number;
  fragLength: number;
}
export interface LoadedTexture {
  type: 'LoadedTexture'
  resourceID: number;
  pointer: number;
  width: number;
  height: number;
}
export interface MissingTexture {
  type: 'MissingTexture'
  resourceID: number;
}
export interface LoadedBSP {
  type: 'LoadedBSP'
  resourceID: number;
  pointer: number;
}
export interface LoadedTextureOptions {
  type: 'LoadedTextureOptions'
  resourceID: number;
  surfaceParamTrans: boolean;
}
export type Message = { type: 'Unknown' }  | TestMessage  | CPPLoaded  | LoadResource  | LoadShaders  | LoadedShaders  | LoadedTexture  | MissingTexture  | LoadedBSP  | LoadedTextureOptions 
export function parseMessage(json: string): Message {
  const val = JSON.parse(json)
  switch (val.type) {
    case 'TestMessage': return val as TestMessage
    case 'CPPLoaded': return val as CPPLoaded
    case 'LoadResource': return val as LoadResource
    case 'LoadShaders': return val as LoadShaders
    case 'LoadedShaders': return val as LoadedShaders
    case 'LoadedTexture': return val as LoadedTexture
    case 'MissingTexture': return val as MissingTexture
    case 'LoadedBSP': return val as LoadedBSP
    case 'LoadedTextureOptions': return val as LoadedTextureOptions
  }
  return { type: 'Unknown' }
}