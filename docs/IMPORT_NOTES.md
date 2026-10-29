# 首次导入说明

日期：2026-10-29。

## 保留

- 根目录 `project.uvprojx`、`Project/` 源码和 `RTE/_Target_1/RTE_Components.h`。
- `实例工程/` 的 01 至 16 号 HAL 示例，以及总览说明。
- `HAL标准历程_MiniFreeRTOS/综合项目_MiniFreeRTOS多任务演示站/` 的源码、依赖和原说明文档。
- `docs/MiniFreeRTOS_探索者F407_循序渐进项目计划.docx`。

保留原目录和项目相对路径，各工程第三方驱动和源码中的版权声明保持原样。内核和应用源码未做功能修改。

## 排除

- `实例工程_备份_旧版/`。
- `实例工程/16_TFT显示RAIN_broken/` 和旧寄存器版 `16_TFT显示RAIN_Mini/`。
- 示例 04 根目录残留的旧 `project.uvprojx`：其引用的旧 `Project/` 已不存在，应使用该示例下 `Projects/MDK-ARM/atk_f407.uvprojx`。
- 整套 HAL 标准实验全集，仅选择综合演示工程。
- Objects、Listings、Output、DebugConfig 及调试会话、构建日志、编译产物。
- 临时脚本、修复报告、Office 锁文件和清理批处理。
- 指向未导入 HAL 实验全集的迁移提示文件。

## 检查范围

上传前检查所有保留的 Keil `.uvprojx` 能解析为 XML，且项目 `FilePath` 指向的本地文件全部存在；检查源码与原件的 SHA-256 一致，排除构建输出及常见凭据模式。

首次导入检查结果：共 4,052 个文件、18 个 Keil 工程；469 处工程文件引用全部存在；4,047 个复制文件的 SHA-256 与原件一致；其余 5 个为新增 README、审查/整理说明和 Git 配置文件。文件原始总量约 196.91 MiB，Git 对重复依赖进行内容去重。

这些检查只验证整理完整性，不证明程序可编译或能在硬件上正确运行。本次未运行 Keil 编译和上板测试，当前内核问题见 `KERNEL_REVIEW.md`。
