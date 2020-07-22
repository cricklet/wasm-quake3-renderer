import os, json
from pprint import pprint

all_textures = {}

for subdir, dirs, files in os.walk('data/textures'):
    for file in files:
      all_textures[os.path.join(subdir, file)] = True

texture_to_shader = {}

for subdir, dirs, files in os.walk('data/scripts'):
    for file in files:
      f = open(os.path.join(subdir, file), 'r', encoding="utf8", errors='ignore')

      current_texture = None
      braces = 0
      shader_source = []

      for line in f:
        line = line.strip()
        if line.startswith('//') or len(line) == 0:
          continue
        if current_texture:
          if line == '{':
            braces += 1
            if braces == 2:
              shader_source.append([])
          elif line == '}':
            braces -= 1
          else:
            if braces == 1:
              shader_source.append(line.split(' '))
            elif braces == 2:
              shader_source[-1].append(line.split(' '))
            else:
              assert False
          if braces == 0:
            texture_to_shader[current_texture] = shader_source
            current_texture = None
        if line.startswith('textures'):
          current_texture = line
          braces = 0
          shader_source = []

file = open('./data/textures_manifest.json', 'w')
file.truncate(0)
file.write(json.dumps({
  'textures': all_textures,
  'shaders': texture_to_shader
}))
file.close()
