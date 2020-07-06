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
export interface LoadedImage {
  type: 'LoadedImage'
  resourceID: number;
  pointer: number;
  width: number;
  height: number;
}
export interface LoadedBSP {
  type: 'LoadedBSP'
  resourceID: number;
  pointer: number;
}
export type Message = { type: 'Unknown' }  | TestMessage  | CPPLoaded  | LoadResource  | LoadShaders  | LoadedShaders  | LoadedImage  | LoadedBSP 
export function parseMessage(json: string): Message {
  const val = JSON.parse(json)
  switch (val.type) {
    case 'TestMessage': return val as TestMessage
    case 'CPPLoaded': return val as CPPLoaded
    case 'LoadResource': return val as LoadResource
    case 'LoadShaders': return val as LoadShaders
    case 'LoadedShaders': return val as LoadedShaders
    case 'LoadedImage': return val as LoadedImage
    case 'LoadedBSP': return val as LoadedBSP
  }
  return { type: 'Unknown' }
}