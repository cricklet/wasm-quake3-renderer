export enum ResourceType {
  BSP_FILE,
  IMAGE_FILE,
  UNKNOWN
};
export interface TestMessage {
  type: 'TestMessage'
  text: string;
}
export interface TestPointer {
  type: 'TestPointer'
  pointer: any;
}
export interface OSXReady {
  type: 'OSXReady'
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
  vertPointer: any;
  fragPointer: any;
  vertLength: number;
  fragLength: number;
}
export interface LoadedTexture {
  type: 'LoadedTexture'
  resourceID: number;
  pointer: any;
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
  pointer: any;
}
export interface LoadedTextureOptions {
  type: 'LoadedTextureOptions'
  resourceID: number;
  surfaceParamTrans: boolean;
}
export type Message = { type: 'Unknown' }  | TestMessage  | TestPointer  | OSXReady  | LoadResource  | LoadShaders  | LoadedShaders  | LoadedTexture  | MissingTexture  | LoadedBSP  | LoadedTextureOptions 
export function parseMessage(json: string): Message {
  const val = JSON.parse(json)
  switch (val.type) {
    case 'TestMessage': return val as TestMessage
    case 'TestPointer': return val as TestPointer
    case 'OSXReady': return val as OSXReady
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