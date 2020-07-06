
enums = [
  ('ResourceType', 'RESOURCE_', ['BSP_FILE', 'IMAGE_FILE'])
]

messages = [
  ('TestMessage', [
    ('text', 'string')
  ]),

  ('CPPLoaded', []),

  ('LoadResource', [
    ('url', 'string'),
    ('resourceType', 'ResourceType'),
    ('resourceID', 'int')
  ]),

  ('LoadShaders', [
    ('vertUrl', 'string'),
    ('fragUrl', 'string'),
    ('resourceID', 'int')
  ]),

  ('LoadedShaders', [
    ('resourceID', 'int'),
    ('vertPointer', 'void*'),
    ('fragPointer', 'void*'),
  ]),

  ('LoadedImage', [
    ('resourceID', 'int'),
    ('pointer', 'void*'),
    ('width', 'int'),
    ('height', 'int')
  ]),

  ('LoadedBSP', [
    ('resourceID', 'int'),
    ('pointer', 'void*')
  ])
]