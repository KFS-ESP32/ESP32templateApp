export interface SystemStatus {
    // HardwareInfo
    chipmodel: string;
    chiprevision: number;
    chipcores: number;
    cpufreq: number;
    // FirmwareInfo
    hostname: string;
    sdkversion: string;
    config_version: string;
    pioenv: string;
    resetreason_0: string;
    resetreason_1: string;
    cfgsavecount: number;
    uptime: number;
    // MemoryInfo
    heap_total: number;
    heap_used: number;
    heap_max_block: number;
    heap_min_free: number;
    littlefs_total: number;
    littlefs_used: number;
    sketch_total: number;
    sketch_used: number;
}
