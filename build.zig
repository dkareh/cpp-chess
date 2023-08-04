const std = @import("std");

pub fn build(b: *std.Build) void {
    // Let the person running `zig build` choose the target to build for.
    const target = b.standardTargetOptions(.{});

    // Let the person running `zig build` choose the optimization mode.
    const optimize = b.standardOptimizeOption(.{});

    const exe_cflags = [_][]const u8{
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-pedantic",
    };

    const exe = b.addExecutable(.{
        .name = if (optimize == .Debug) "chess-d" else "chess",
        .target = target,
        .optimize = optimize,
    });

    exe.linkLibC();
    exe.linkLibCpp();
    exe.addIncludePath("include");
    exe.addCSourceFiles(&.{
        "src/displays/AsciiDisplay.cpp",
        "src/displays/LetterDisplay.cpp",
        "src/displays/TwoLetterDisplay.cpp",
        "src/Board.cpp",
        "src/chess960.cpp",
        "src/Display.cpp",
        "src/Game.cpp",
        "src/main.cpp",
        "src/Menu.cpp",
        "src/Piece.cpp",
        "src/safe_ctype.cpp",
    }, &exe_cflags);

    if (exe.target.getOsTag() == .windows) {
        exe.addCSourceFile("src/displays/WindowsConsoleDisplay.cpp", &exe_cflags);
        exe.defineCMacro("CHESS_ON_WINDOWS", null);

        // FIXME: https://github.com/ziglang/zig/issues/15958
        exe.want_lto = false;
    }

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Play chess");
    run_step.dependOn(&run_cmd.step);
}