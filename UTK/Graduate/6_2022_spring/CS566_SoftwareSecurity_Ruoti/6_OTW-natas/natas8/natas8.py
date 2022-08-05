import base64

hexSecret = "3d3d516343746d4d6d6c315669563362"
byteSecret = bytes.fromhex(hexSecret)
byteSecret = byteSecret[::-1]
byteSecret = base64.decodebytes(byteSecret)

print(byteSecret)
