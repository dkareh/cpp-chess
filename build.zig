const std = @import("std");
const Build = std.Build;

pub fn build(b: *Build) void {
    // Let the person running `zig build` choose the target.
    const target = b.standardTargetOptions(.{});

    // Let the person running `zig build` choose the optimization mode.
    const optimize = b.standardOptimizeOption(.{});

    const exe_cflags = [_][]const u8{
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "-Wconversion",
    };

    const exe = b.addExecutable(.{
        .name = if (optimize == .Debug) "chess-d" else "chess",
        .target = target,
        .optimize = optimize,
    });

    exe.linkLibC();
    exe.linkLibCpp();
    exe.addCSourceFiles(.{ .files = &.{
        "src/Board.cpp",
        "src/chess960.cpp",
        "src/Game.cpp",
        "src/main.cpp",
        "src/Menu.cpp",
        "src/Piece.cpp",
        "src/safe_ctype.cpp",
        "src/TerminalUserInterface.cpp",
        "src/ui/AsciiUi.cpp",
        "src/ui/LetterUi.cpp",
        "src/ui/TwoLetterUi.cpp",
    }, .flags = &exe_cflags });

    if (exe.rootModuleTarget().os.tag == .windows) {
        exe.addCSourceFiles(.{ .files = &.{"src/ui/WindowsConsoleUi.cpp"}, .flags = &exe_cflags });
        exe.defineCMacro("CHESS_ON_WINDOWS", null);
    }

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Play chess");
    run_step.dependOn(&run_cmd.step);
}
