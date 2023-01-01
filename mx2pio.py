Import("env")

env.AddCustomTarget(
    name="mxupd",
    dependencies = None,
    actions=[
        "mv src Src",
        "mv include Inc",
        "stm32pio generate",
        "mv Inc include",
        "mv Src src"
    ],
    title="CubeMX generate",
    description="Update initialization code from .ioc file",
    always_build=True
)