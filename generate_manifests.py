import os, json

result = {}

for subdir, dirs, files in os.walk('data/textures'):
    for file in files:
      result[os.path.join(subdir, file)] = True

file = open('./data/textures_manifest.json', 'w')
file.truncate(0)
file.write(json.dumps(result))
file.close()
