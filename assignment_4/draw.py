from PIL import Image, ImageDraw

lines = open('pixels.txt', 'r').readlines()
lines = [[int(tmp1) for tmp1 in tmp.replace('\n', '').strip().split(' ')] for tmp in lines]

im = Image.new('RGB', (len(lines), len(lines[0])), (0, 0, 0))
draw = ImageDraw.Draw(im)

for x in range(0, len(lines)):
    for y in range(0, len(lines[0])):
        draw.point([x, y], (lines[x][y], lines[x][y], lines[x][y]))

im.save('out.png', 'PNG')