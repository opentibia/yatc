# -*-Python-*-

config_setting(
    name = "darwin",
    values = {"host_cpu": "darwin"}
)

genrule(
    name = "translations",
    srcs = select(
        {
          "//conditions:default": [
            "es_ES.po",
            "hr_HR.po",
            "pl_PL.po",
            "pt_BR.po",
            "sv_SE.po",
          ],
          ":darwin": [],
         }),
    outs = #select(
    #    {
    #      "//conditions:default": 
           [
            "es_ES/LC_MESSAGES/yatc.mo",
            "hr_HR/LC_MESSAGES/yatc.mo",
            "pl_PL/LC_MESSAGES/yatc.mo",
            "pt_BR/LC_MESSAGES/yatc.mo",
            "sv_SE/LC_MESSAGES/yatc.mo",
          ],
    #      ":darwin": [],
    #    }),
    tools = ["//translations:build.sh"],
    cmd = "\n".join([
        #"cd $$(dirname $(location //translations:build.sh))",
        #"./build.sh $(@D)",
        "$(location //translations:build.sh) $(@D)/",
    ]),
    visibility = ["//visibility:public"],
)
