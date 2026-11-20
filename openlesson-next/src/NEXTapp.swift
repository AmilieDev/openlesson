
import SwiftUI

enum SidebarItem: String, Hashable {
    case overview
    case mountEFI
    case installNVIDIA
    case systemPatches
    case settings
}

@main
struct NEXT: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}

struct ContentView: View {
    @State private var selection: SidebarItem? = .overview

    var body: some View {
        NavigationSplitView {
            List(selection: $selection) {
                NavigationLink(value: SidebarItem.overview) {
                    Label("Overview", systemImage: "house")
                }

                Section("Utilities") {
                    NavigationLink(value: SidebarItem.mountEFI) {
                        Label("Mount EFI", systemImage: "externaldrive")
                    }

                    NavigationLink(value: SidebarItem.installNVIDIA) {
                        Label("Install NVIDIA", systemImage: "display")
                    }

                    NavigationLink(value: SidebarItem.systemPatches) {
                        Label("System Patches", systemImage: "wrench")
                    }
                }

                Section("Application") {
                    NavigationLink(value: SidebarItem.settings) {
                        Label("Settings", systemImage: "gear")
                    }
                }
            }
            .navigationTitle("OpenLesson NEXT")
            .listStyle(.sidebar)
        } detail: {
            Group {
                switch selection {
                case .overview:
                    Text("Welcome to OpenLesson NEXT")

                case .mountEFI:
                    Text("Mount EFI Utility")

                case .installNVIDIA:
                    Text("NVIDIA Driver Installer")

                case .systemPatches:
                    Text("System Patches")

                case .settings:
                    Text("Settings")

                case nil:
                    ContentUnavailableView(
                        "Select a Utility",
                        systemImage: "sidebar.left"
                    )
                }
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity)
        }
        .frame(minWidth: 850, minHeight: 550)
    }
}
