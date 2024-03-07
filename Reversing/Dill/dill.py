class Dill:
    prefix = 'sun{'
    suffix = '}'
    o = [5,1,3,4,7,2,6,0]

    def __init__(self) -> None:
        # value = flag[len(Dill.prefix):-len(Dill.suffix)]
        # c = [value[i:i+4] for i in range(0, len(value), 4)]
        # order = [5,1,3,4,7,2,6,0]
        # self.encrypted = ''.join([c[i] for i in order])
        self.encrypted = 'bGVnbGxpaGVwaWNrdD8Ka2V0ZXRpZGls'

    def validate(self, value: str) -> bool:
        if not value.startswith(Dill.prefix) or not value.endswith(Dill.suffix):
            return False

        value = value[len(Dill.prefix):-len(Dill.suffix)]

        if len(value) != 32:
            return False

        # split value into 4 character chunks
        c = [value[i:i+4] for i in range(0, len(value), 4)]

        # rearrange chunks
        value = ''.join([c[i] for i in Dill.o])

        if value != self.encrypted:
            return False

        return True
