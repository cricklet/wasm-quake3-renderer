
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
    ('vertLength', 'int'),
    ('fragLength', 'int'),
  ]),

  ('LoadedTexture', [
    ('resourceID', 'int'),
    ('pointer', 'void*'),
    ('width', 'int'),
    ('height', 'int')
  ]),

  ('MissingTexture', [
    ('resourceID', 'int')
  ]),

  ('LoadedBSP', [
    ('resourceID', 'int'),
    ('pointer', 'void*')
  ])
]