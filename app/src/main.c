#include "scrcpy.h"

#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <libavformat/avformat.h>
#define SDL_MAIN_HANDLED // avoid link error on Linux Windows Subsystem
#include <SDL2/SDL.h>

#include "config.h"
#include "cli.h"
#include "compat.h"
#include "util/log.h"

// 这个方法在执行scrcpy -v 的时候会调用，主要负责打印scrcpy的相关信息
static void
print_version(void) {
    // 打印 scrcpy 的版本
    fprintf(stderr, "scrcpy %s\n\n", SCRCPY_VERSION);

    // 打印相关依赖的版本的版本
    fprintf(stderr, "dependencies:\n");
    fprintf(stderr, " - SDL %d.%d.%d\n", SDL_MAJOR_VERSION, SDL_MINOR_VERSION,
                                         SDL_PATCHLEVEL);
    fprintf(stderr, " - libavcodec %d.%d.%d\n", LIBAVCODEC_VERSION_MAJOR,
                                                LIBAVCODEC_VERSION_MINOR,
                                                LIBAVCODEC_VERSION_MICRO);
    fprintf(stderr, " - libavformat %d.%d.%d\n", LIBAVFORMAT_VERSION_MAJOR,
                                                 LIBAVFORMAT_VERSION_MINOR,
                                                 LIBAVFORMAT_VERSION_MICRO);
    fprintf(stderr, " - libavutil %d.%d.%d\n", LIBAVUTIL_VERSION_MAJOR,
                                               LIBAVUTIL_VERSION_MINOR,
                                               LIBAVUTIL_VERSION_MICRO);
}

static SDL_LogPriority
convert_log_level_to_sdl(enum sc_log_level level) {
    switch (level) {
        case SC_LOG_LEVEL_DEBUG:
            return SDL_LOG_PRIORITY_DEBUG;
        case SC_LOG_LEVEL_INFO:
            return SDL_LOG_PRIORITY_INFO;
        case SC_LOG_LEVEL_WARN:
            return SDL_LOG_PRIORITY_WARN;
        case SC_LOG_LEVEL_ERROR:
            return SDL_LOG_PRIORITY_ERROR;
        default:
            assert(!"unexpected log level");
            return SDL_LOG_PRIORITY_INFO;
    }
}


int
main(int argc, char *argv[]) {
#ifdef __WINDOWS__
    // disable buffering, we want logs immediately
    // even line buffering (setvbuf() with mode _IOLBF) is not sufficient
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
#endif
    // scrcpy_cli_args即为scrcpy客户端的参数，是一个结构体
    struct scrcpy_cli_args args = {
        .opts = SCRCPY_OPTIONS_DEFAULT,
        .help = false,
        .version = false,
    };

#ifndef NDEBUG
    // 如果是 debug 模式，设置 log 的等级
    args.opts.log_level = SC_LOG_LEVEL_DEBUG;
#endif
    // 利用 scrcpy_parse_args 自定义的参数解析 main 接收到的参数，并将 args 的地址交给 scrcpy_parse_args 函数
    // 意味着 scrcpy_parse_args 函数解析后直接更改了 args 这个结构体
    if (!scrcpy_parse_args(&args, argc, argv)) {
        // 解析失败直接退出
        return 1;
    }
    // 将解析后的 args 的日志等级转换成 SDL 的日志等级
    SDL_LogPriority sdl_log = convert_log_level_to_sdl(args.opts.log_level);
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, sdl_log);
    // 如果解析出的 args.help 为真，即用户输入了 scrcpy -h，就执行打印帮助函数
    if (args.help) {
        scrcpy_print_usage(argv[0]);
        return 0;
    }

    if (args.version) {
        print_version();
        return 0;
    }
    // 打印了 scrcpy 的 info 
    LOGI("scrcpy " SCRCPY_VERSION " <https://github.com/Genymobile/scrcpy>");

#ifdef SCRCPY_LAVF_REQUIRES_REGISTER_ALL
    调用了 ffmpeg 库的一个函数，为后续解码做准备
    av_register_all();
#endif
    // 初始化 ffmpeg 网络模块
    if (avformat_network_init()) {
        return 1;
    }

    int res = scrcpy(&args.opts) ? 0 : 1;

    // 释放 ffmpeg 网络模块
    avformat_network_deinit(); // ignore failure

    return res;
}
