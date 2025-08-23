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
        "-Wno-sign-conversion",
    };

    const mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
        .link_libcpp = true,
    });

    mod.addCSourceFiles(.{ .files = &.{
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

    if (mod.resolved_target.?.result.os.tag == .windows) {
        mod.addCSourceFiles(.{ .files = &.{"src/ui/WindowsConsoleUi.cpp"}, .flags = &exe_cflags });
        mod.addCMacro("CHESS_ON_WINDOWS", "1");
    }

    const exe = b.addExecutable(.{
        .name = if (optimize == .Debug) "chess-d" else "chess",
        .root_module = mod,
    });
    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Play chess");
    run_step.dependOn(&run_cmd.step);
}
