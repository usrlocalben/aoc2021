load("//:copts.bzl", "RQDQ_DEFAULT_COPTS")

package(default_visibility = ["//visibility:public"])

# cc_binary(name = "02",
#           srcs = ["02.cxx"],
#           copts = RQDQ_DEFAULT_COPTS,
#           data = ["//data/02:input.txt",],
#           deps = ["//3rdparty/fast_io"])

cc_library(name = "lib",
           srcs = ["lib.cxx"],
           hdrs = ["lib.hxx"],
           copts = RQDQ_DEFAULT_COPTS)

[cc_binary(name = n,
          srcs = ["%s.cxx" % n],
          copts = RQDQ_DEFAULT_COPTS,
          deps = ["//3rdparty:backward",
                  "//3rdparty:fast_io",
                  ":lib"])
 for n in ('01', '02', '03', '04', '05',
           '06', '07', '08', '09', '10',
           '11', '12', '13', '14', '15',
           '16', '17', '18', '19', '20',
           '21', '22', '23', '24', '25')]
